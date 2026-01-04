import LobbyBar from "@/Components/LobbyBar";
import { mainStore } from "@/store";
import { useMemo } from "react";
import { useStore } from "zustand/react";
import LoadingLobby from "./Components/LoadingLobby";
import PlayersList from "./Components/PlayersList";

export default function LobbyPage() {
  const gameInfo = useStore(mainStore, (state) => state.game.gameInfo);

  const requiredPlayers = useMemo(() => (gameInfo?.players.length ?? 0) >= 2, [gameInfo]);

  return (
    <div className="h-full flex flex-col gap-4">
      <LobbyBar />

      <div className="flex-1 overflow-hidden">
        {gameInfo === null ? <LoadingLobby /> : <PlayersList players={gameInfo.players} />}
      </div>

      <div className="flex justify-between gap-4">
        <button
          disabled={!requiredPlayers}
          className="btn-success flex-1"
          onClick={() => window.bridges?.game.callStartGame()}
        >
          Start Game {!requiredPlayers && "(Need 2+ players)"}
        </button>

        <button 
          className="btn-danger"
          onClick={() => window.bridges?.game.callQuitGame()}
        >
          Leave
        </button>
      </div>
    </div>
  );
}
