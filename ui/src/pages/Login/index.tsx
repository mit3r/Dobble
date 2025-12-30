import LobbyServerConnectionStatus from "@/Components/LobbyServerConnectionStatus";
import { mainStore } from "@/store";
import { CommunicationStatus } from "@/types/dobble";
import { useCallback, useMemo, useState } from "react";
import { useStore } from "zustand";

export default function LoginPage() {
  const [value, setValue] = useState<string>("");

  const lobbyCommunication = useStore(mainStore, (state) => state.browser.lobbyCommunication);
  const inVerification = useMemo(
    () => lobbyCommunication === CommunicationStatus.Waiting,
    [lobbyCommunication]
  );

  const error = useStore(mainStore, (state) => state.browser.nicknameError);

  const handleSubmit = useCallback(() => {
    if (inVerification) return;
    window.bridges?.browser.callVerifyNickname(value);
  }, [inVerification, value]);

  const handleChange = useCallback(
    (e: React.ChangeEvent<HTMLInputElement>) => {
      if (inVerification) return;
      setValue(e.target.value);
    },
    [inVerification, setValue]
  );

  return (
    <div className="flex flex-col items-center gap-2">
      <LobbyServerConnectionStatus />

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
          className="mt-4 p-2 border-2 disabled:animate-pulse duration-300"
          onClick={handleSubmit}
          disabled={inVerification}
        >
          Login
        </button>

        {error && <p className="text-red-500 mt-2">{error}</p>}
      </>
    </div>
  );
}
