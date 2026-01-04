import { GameStatusSpan } from "@/Components/Statuses";
import { mainStore } from "@/store";
import { useStore } from "zustand/react";
import Loading from "../Loading";

export default function LobbyBar() {
  const gameInfo = useStore(mainStore, (state) => state.game.gameInfo);

  return (
    <div className="flex items-center justify-between p-4 rounded-xl bg-dobble-card/50 backdrop-blur-sm">
      <div className="flex-1 text-left">
        <span className="text-sm text-dobble-text-muted">Room</span>
        <h2 className="text-xl font-bold text-dobble-text">
          {!gameInfo ? <Loading /> : gameInfo.gameId}
        </h2>
      </div>
      
      <h1 className="flex-1 page-title text-3xl">Dobble</h1>
      
      <div className="flex-1 text-right">
        <span className="text-sm text-dobble-text-muted">Status</span>
        <h2 className="text-xl font-bold">
          {!gameInfo ? <Loading /> : <GameStatusSpan status={gameInfo.status} />}
        </h2>
      </div>
    </div>
  );
}
