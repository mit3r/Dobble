import { mainStore } from "@/store";
import { useCallback, useState } from "react";
import { useStore } from "zustand";

export default function NicknameComponent() {
  const [value, setValue] = useState<string>("");

  const isBlocked = useStore(mainStore, (state) => state.browser.isBlocked());
  const error = useStore(mainStore, (state) => state.main.nicknameError);

  const handleSubmit = useCallback(() => {
    window.bridges?.browser.callVerifyNickname(value);
  }, [value]);

  const handleChange = useCallback(
    (e: React.ChangeEvent<HTMLInputElement>) => setValue(e.target.value),
    [setValue]
  );

  return (
    <div className="p-4 px-8 border-2 rounded-xl flex flex-col gap-4 items-center">
      <p className="w-fit">Please enter your nickname to start playing.</p>

      <input
        className="p-2 border-2 rounded"
        type="text"
        placeholder="Enter your nickname"
        value={value}
        onChange={handleChange}
      />

      <button
        className="p-2 px-8 border-2 disabled:opacity-50"
        onClick={handleSubmit}
        disabled={isBlocked}
      >
        Login
      </button>

      {error && <p className="text-red-500 mt-2">{error}</p>}
    </div>
  );
}
