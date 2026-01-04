import  { mainStore } from "@/store";
import  { useStore } from "zustand/react";
import ServerStatus from ".";
import { ConnectionStatus } from "@/types/dobble";

export default function GameServStatus() {
  const connectionStatus = useStore(mainStore, (state) => state.game.gameServerConnectionStatus);
  const connectionError = useStore(mainStore, (state) => state.game.gameServerConnectionError);

  if (connectionStatus === ConnectionStatus.Connected) return null;
  if (connectionError === null) return null;

  return (
    <div className="w-full flex">
      <div className="shrink-0 p-2">Game server: </div>
      <ServerStatus connectionStatus={connectionStatus} connectionError={connectionError} />
    </div>
  );
}