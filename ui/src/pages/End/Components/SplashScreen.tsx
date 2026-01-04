import { mainStore } from "@/store";
import type { PlayerGameInfo } from "@/types/dobble";
import { useMemo } from "react";
import { useStore } from "zustand/react";

export default function SplashScreen({ sortedPlayers }: { sortedPlayers: PlayerGameInfo[] }) {
  const nickname = useStore(mainStore, (state) => state.main.nickname);

  const index = sortedPlayers.findIndex((player) => player.nickname === nickname);

  const { prompt, color } = useMemo(() => {
    switch (index) {
      case -1:
        return { prompt: "You watched this game", color: "text-dobble-text-muted" };
      case 0:
        return { prompt: "Congratulations! You are the winner!", color: "text-dobble-accent" };
      case 1:
        return { prompt: "Great job! You came in second place!", color: "text-dobble-primary" };
      case 2:
        return { prompt: "Well played! You secured third place!", color: "text-dobble-secondary" };
      case sortedPlayers.length - 1:
        return { prompt: "Don't give up! Better luck next time!", color: "text-dobble-text-muted" };
      default:
        return { prompt: `You finished in position ${index + 1}. Better luck next time!`, color: "text-dobble-text" };
    }
  }, [index, sortedPlayers.length]);

  return (
    <div className="panel flex flex-col items-center justify-center gap-6 text-center">
      <h1 className="text-4xl font-bold text-dobble-text">Game Over</h1>
      <p className={`text-xl ${color}`}>{prompt}</p>
      
      {index === 0 && sortedPlayers[0] && (
        <div className="mt-4 p-4 rounded-xl bg-dobble-accent/20">
          <p className="text-dobble-accent font-bold text-2xl">
            {sortedPlayers[0].points} Points
          </p>
        </div>
      )}
    </div>
  );
}
