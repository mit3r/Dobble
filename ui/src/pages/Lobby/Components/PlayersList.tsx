import type { PlayerGameInfo } from "@/types/dobble";

export default function PlayersList({players}: {players: PlayerGameInfo[]}) {
  return (
    <div className="panel h-full flex flex-col">
      <div className="flex items-center justify-between mb-4">
        <h2 className="text-xl font-bold text-dobble-text">Players in Lobby</h2>
        <span className="badge badge-waiting">{players.length} / 4</span>
      </div>

      <ol className="overflow-y-auto flex-1 custom-scrollbar space-y-2">
        {players.map((player) => (
          <li
            key={player.nickname}
            className="flex items-center gap-4 p-4 rounded-xl bg-dobble-card hover:bg-dobble-card-hover transition-colors"
          >
            <div className="avatar">{player.nickname.charAt(0).toUpperCase()}</div>
            <div className="flex-1">
              <span className="font-bold text-dobble-text">{player.nickname}</span>
            </div>
          </li>
        ))}
      </ol>

      {players.length < 2 && (
        <div className="mt-4 p-4 rounded-xl bg-dobble-accent/20 text-center">
          <p className="text-dobble-accent">Waiting for more players to join...</p>
        </div>
      )}
    </div>
  );
}