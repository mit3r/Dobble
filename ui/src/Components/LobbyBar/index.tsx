import { mainStore } from "@/store";
import { GameStatus } from "@/types/dobble";
import { useMemo } from "react";
import { useStore } from "zustand/react";
import Loading from "../Loading";

export default function LobbyBar() {
  const gameInfo = useStore(mainStore, (state) => state.game.gameInfo);

  const redableStatus = useMemo(() => {
    if (!gameInfo) return <Loading />;

    switch (gameInfo.status) {
      case GameStatus.Waiting:
        return "Waiting";
      case GameStatus.InGame:
        return "In Progress";
      case GameStatus.Finished:
        return "Finished";
      default:
        return "Unknown";
    }
  }, [gameInfo]);

  return (
    <div className="flex items-center justify-center relative h-fit py-4 w-full border-y-2">
      <h2 className="flex-1 basis-0 text-xl text-left ">
        Room {!gameInfo ? <Loading /> : gameInfo.gameName}
      </h2>
      <h1 className="flex-1 basis-0 text-4xl font-bold text-center">Dobble</h1>
      <h2 className="flex-1 basis-0 text-xl text-right">{redableStatus}</h2>
    </div>
  );
}
