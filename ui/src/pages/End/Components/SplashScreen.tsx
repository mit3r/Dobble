import { mainStore } from "@/store";
import type { PlayerGameInfo } from "@/types/dobble";
import { useMemo } from "react";
import { useStore } from "zustand/react";

export default function SplashScreen({ sortedPlayers }: { sortedPlayers: PlayerGameInfo[] }) {
  const nickname = useStore(mainStore, (state) => state.main.nickname);

  const index = sortedPlayers.findIndex((player) => player.nickname === nickname);

  const prompt = useMemo(() => {
    switch (index) {
      case -1:
        return "You are not ranked in this game.";
      case 0:
        return "Congratulations! You are the winner!";
      case 1:
        return "Great job! You came in second place!";
      case 2:
        return "Well played! You secured third place!";
      case sortedPlayers.length - 1:
        return "Don't be discouraged! Better luck next time!";
      default:
        return `You finished in position ${index + 1}. Better luck next time!`;
    }
  }, [index, sortedPlayers.length]);

  return (
    <div className="flex flex-col items-center h-full gap-4 p-8 text-center">
      <h1 className="text-3xl font-bold">Game Over</h1>
      <p className="text-xl">{prompt}</p>
    </div>
  );
}
