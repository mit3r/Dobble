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

  const handleKeyDown = useCallback(
    (e: React.KeyboardEvent<HTMLInputElement>) => {
      if (e.key === "Enter" && !isBlocked && value.trim()) {
        handleSubmit();
      }
    },
    [handleSubmit, isBlocked, value]
  );

  return (
    <div className="flex flex-col gap-6">
      <p className="text-center text-dobble-text-muted">
        Please enter your nickname to start playing.
      </p>

      <div className="flex flex-col gap-2">
        <label htmlFor="nickname" className="text-sm font-bold text-dobble-text-muted">
          Nickname
        </label>
        <input
          id="nickname"
          className="input-field w-full"
          type="text"
          placeholder="Enter your nickname..."
          value={value}
          onChange={handleChange}
          onKeyDown={handleKeyDown}
          autoFocus
        />
      </div>

      <button
        className="btn-primary w-full"
        onClick={handleSubmit}
        disabled={isBlocked || !value.trim()}
      >
        Start Playing
      </button>

      {error && (
        <div className="text-center p-3 rounded-xl bg-dobble-danger/20 text-dobble-danger">
          {error}
        </div>
      )}
    </div>
  );
}
