import LobbyBar from "@/Components/LobbyBar";
import QuitFooter from "@/Components/QuitFooter";
import { mainStore } from "@/store";
import { useMemo } from "react";
import { useStore } from "zustand/react";
import LoadingScoreboard from "./Components/LoadingScoreboard";
import Scoreboard from "./Components/Scoreboard";
import SplashScreen from "./Components/SplashScreen";

export default function EndPage() {
  const players = useStore(mainStore, (state) => state.game.gameInfo?.players);
  const sortedPlayers = useMemo(
    () => (players ? [...players].sort((a, b) => b.points - a.points) : null),
    [players]
  );

  return (
    <div className="h-full flex flex-col gap-4">
      <LobbyBar />

      {sortedPlayers ? (
        <div className="flex-1 grid grid-cols-1 lg:grid-cols-[1fr_2fr] gap-4 overflow-hidden">
          <SplashScreen sortedPlayers={sortedPlayers} />
          <Scoreboard sortedPlayers={sortedPlayers} />
        </div>
      ) : (
        <LoadingScoreboard />
      )}

      <QuitFooter label="Leave Game" onQuit={() => window.bridges?.game.callQuitGame()} />
    </div>
  );
}
