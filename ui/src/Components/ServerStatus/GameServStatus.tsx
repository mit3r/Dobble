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
    <div className="w-full flex justify-center">
      <span className="w-fit">Game server: </span>
      <ServerStatus connectionStatus={connectionStatus} connectionError={connectionError} />
    </div>
  );
}