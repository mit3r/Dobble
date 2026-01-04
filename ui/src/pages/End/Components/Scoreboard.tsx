import { mainStore } from "@/store";
import type { PlayerGameInfo } from "@/types/dobble";
import { useMemo } from "react";
import { useStore } from "zustand";

export default function Scoreboard({ sortedPlayers }: { sortedPlayers: PlayerGameInfo[] }) {
  const nickname = useStore(mainStore, (state) => state.main.nickname);

  const playerIndex = useMemo(
    () => sortedPlayers.findIndex((player) => player.nickname === nickname),
    [sortedPlayers, nickname]
  );

  const getRankEmoji = (index: number) => {
    switch (index) {
      case 0: return "#1";
      case 1: return "#2";
      case 2: return "#3";
      default: return `#${index + 1}`;
    }
  };

  return (
    <div className="panel flex flex-col flex-1 min-h-0 overflow-hidden">
      <h2 className="text-xl font-bold mb-4 text-dobble-text flex items-center gap-2">
        Scoreboard
      </h2>
      
      <div className="flex-1 overflow-y-auto custom-scrollbar">
        <table className="w-full text-left">
          <thead className="sticky top-0 bg-dobble-card">
            <tr className="text-dobble-text-muted text-sm border-b border-dobble-card-border">
              <th className="p-3">Rank</th>
              <th className="p-3">Player</th>
              <th className="p-3 text-right">Points</th>
              <th className="p-3 text-right">Matches</th>
              <th className="p-3 text-right">Mistakes</th>
            </tr>
          </thead>
          <tbody>
            {sortedPlayers.map((player, index) => (
              <tr 
                key={player.nickname}
                className={`
                  border-b border-dobble-card-border/50 transition-colors
                  ${index === playerIndex ? 'bg-dobble-primary/20' : 'hover:bg-dobble-card-hover'}
                `}
              >
                <td className="p-3 text-xl">{getRankEmoji(index)}</td>
                <td className="p-3">
                  <span className={`font-bold ${index === playerIndex ? 'text-dobble-primary' : 'text-dobble-text'}`}>
                    {player.nickname}
                  </span>
                  {index === playerIndex && (
                    <span className="ml-2 text-xs text-dobble-primary">(You)</span>
                  )}
                </td>
                <td className="p-3 text-right font-bold text-dobble-success">{player.points}</td>
                <td className="p-3 text-right">{player.matches}</td>
                <td className="p-3 text-right text-dobble-danger">{player.mistakes}</td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
    </div>
  );
}
