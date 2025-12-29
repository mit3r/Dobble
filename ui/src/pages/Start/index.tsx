import { useState } from "react";
import { useStore } from "zustand";
import { mainStore } from "@/store";

export default function StartPage() {
  const [value, setValue] = useState<string>("");

  const nickname = useStore(mainStore, (state) => state.start.nickname);

  return (
    <div className="flex flex-col items-center gap-2">
      <h1 className="text-4xl font-bold mb-4">Welcome to Dobble!</h1>

      {nickname ? (
        <div>
          <p className="text-green-600 font-bold">Logged in as: {nickname}</p>
          <p>Ready to play!</p>
        </div>
      ) : (
        <>
          <p>Please enter your nickname to start playing.</p>

          <input
            type="text"
            placeholder="Enter your nickname"
            value={value}
            onChange={(e) => setValue(e.target.value)}
            className="p-2 border-2 rounded"
          />

          <button
            className="mt-4 p-2 border-2"
            onClick={() => window.bridges?.start.callVerifyNickname(value)}
          >
            Login
          </button>
        </>
      )}
    </div>
  );
}
