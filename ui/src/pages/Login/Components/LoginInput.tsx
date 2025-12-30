import { mainStore } from "@/store";
import { useState, useCallback } from "react";
import { useStore } from "zustand/react";

export default function LoginInput() {
  const [value, setValue] = useState<string>("");

  const inVerification = useStore(mainStore, (state) => state.login.inVerification);
  const verifyNickname = useStore(mainStore, (state) => state.login.verifyNickname);
  const error = useStore(mainStore, (state) => state.login.error);

  const handleSubmit = useCallback(() => {
    if (inVerification) return;
    verifyNickname(value);
  }, [inVerification, value, verifyNickname]);

  const handleChange = useCallback(
    (e: React.ChangeEvent<HTMLInputElement>) => {
      if (inVerification) return;
      setValue(e.target.value);
    },
    [inVerification, setValue]
  );

  return (
    <div className="flex flex-col items-center gap-2">
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