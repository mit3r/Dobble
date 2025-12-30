import { useCallback, useState } from "react";
import { useStore } from "zustand";
import { mainStore } from "@/store";

export default function LoginPage() {
  const connectionStatus = useStore(mainStore, (state) => state.views.lobbyServerStatus);

  return <div className=""></div>;
};
