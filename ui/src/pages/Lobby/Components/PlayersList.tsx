import type { PlayerGameInfo } from "@/types/dobble";

export default function PlayersList({players}: {players: PlayerGameInfo[]}) {
  return (
    <div className="flex flex-col h-full">
      <div className="p-2  text-xl">Players</div>
      <ol className="overflow-y-auto flex-1 border-y-2">
        {players.map((player) => (
          <li key={player.nickname} className="flex justify-between p-2 border-b">
            <span>{player.nickname}</span>
          </li>
        ))}
      </ol>
    </div>
  );
}