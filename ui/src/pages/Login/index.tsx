import LobbyServStatus from "@/Components/ServerStatus/LobbyServStatus";
import { mainStore } from "@/store";
import { useCallback, useState } from "react";
import { useStore } from "zustand";

export default function LoginPage() {
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
    <div className="flex flex-col items-center gap-2">
      <LobbyServStatus />

      <h1 className="text-4xl font-bold mb-4">Welcome to Dobble!</h1>

      <>
        <p>Please enter your nickname to start playing.</p>

        <input
          type="text"
          placeholder="Enter your nickname"
          value={value}
          onChange={handleChange}
          className="p-2 border-2 rounded"
        />

        <button
          className="mt-4 p-2 px-8 border-2 disabled:opacity-50"
          onClick={handleSubmit}
          disabled={isBlocked}
        >
          Login
        </button>

        {error && <p className="text-red-500 mt-2">{error}</p>}
      </>
    </div>
  );
}
