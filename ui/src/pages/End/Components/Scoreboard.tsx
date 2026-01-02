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

  return (
    <div className="flex flex-col flex-1 min-h-0 gap-2">
      <div className="p-2 text-xl border-b-2">Scoreboard</div>
      <table className="text-right text-xl max-w-full overflow-y-scroll">
        <tr>
          <th>Rank</th>
          <th>Nickname</th>
          <th>Points</th>
          <th>Matches</th>
          <th>Mistakes</th>
        </tr>

        {sortedPlayers.map((player, index) => (
          <tr key={player.nickname}>
            <td>{index + 1}</td>
            <td>
              {player.nickname}
              <span className="text-gray-500">{index === playerIndex ? " (You)" : ""}</span>
            </td>
            <td>{player.points}</td>
            <td>{player.matches}</td>
            <td>{player.mistakes}</td>
          </tr>
        ))}
      </table>
    </div>
  );
}
