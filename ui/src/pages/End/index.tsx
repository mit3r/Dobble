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
    () => players?.sort((a, b) => a.points - b.points) || null,
    [players]
  );

  return (
    <div className="h-full flex flex-col justify-end gap-4">
      <LobbyBar />

      {sortedPlayers ? (
        <div className="grid grid-cols-[auto_1fr] gap-4 w-full flex-1">
          <SplashScreen sortedPlayers={sortedPlayers} />
          <Scoreboard sortedPlayers={sortedPlayers} />
        </div>
      ) : (
        <LoadingScoreboard />
      )}

      <QuitFooter label="Leave game!" onQuit={() => window.bridges?.game.callQuitGame()} />
    </div>
  );
}
