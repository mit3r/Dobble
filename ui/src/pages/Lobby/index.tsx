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
    <div className="h-full flex flex-col justify-end gap-4">
      <LobbyBar />

      {gameInfo === null ? <LoadingLobby /> : <PlayersList players={gameInfo.players} />}

      <div className="w-full flex justify-between">
        <button
          disabled={!requiredPlayers}
          className="p-2 px-4 border-2 disabled:opacity-50"
          onClick={() => window.bridges?.game.callStartGame()}
        >
          Start game
        </button>

        <button className="p-2 px-4 border-2" onClick={() => window.bridges?.game.callQuitGame()}>
          Quit
        </button>
      </div>
    </div>
  );
}
