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
    <div className="w-full flex flex-col gap-2">
      <div className="flex items-center gap-2 text-sm text-dobble-text-muted">
        <span>Game Server Status</span>
      </div>
      <ServerStatus connectionStatus={connectionStatus} connectionError={connectionError} />
    </div>
  );
}