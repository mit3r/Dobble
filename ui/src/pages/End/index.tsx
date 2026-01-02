import LobbyBar from "@/Components/LobbyBar";
import QuitFooter from "@/Components/QuitFooter";
import { mainStore } from "@/store";
import { useStore } from "zustand/react";
import LoadingScoreboard from "./Components/LoadingScoreboard";
import Scoreboard from "./Components/Scoreboard";
import SplashScreen from "./Components/SplashScreen";

export default function EndPage() {
  const gameInfo = useStore(mainStore, (state) => state.game.gameInfo);

  return (
    <div className="h-full flex flex-col justify-end gap-4">
      <LobbyBar />

      {gameInfo ? (
        <div className="grid grid-cols-[auto_1fr] gap-4 w-full flex-1">
          <SplashScreen sortedPlayers={gameInfo.players} />
          <Scoreboard sortedPlayers={gameInfo.players} />
        </div>
      ) : (
        <LoadingScoreboard />
      )}

      <QuitFooter label="Leave game!" onQuit={() => {}} />
    </div>
  );
}
