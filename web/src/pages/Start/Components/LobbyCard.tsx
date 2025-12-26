import type { ActualGame } from "@/types";

export default function LobbyCard({ game }: { game: ActualGame }) {
  return (
    <div className="grid grid-cols-1 grid-rows-3 border-2 p-2">
      <h2>{game.gameName}</h2>

      <span>
        Players: {game.players}/{game.maxPlayers}
      </span>

      <div className="flex gap-2 justify-start text-center">
        <button className="p-1 px-4 border-2">Play</button>
        <button className="p-1 px-4 border-2">Spectate</button>

        <span className="m-auto">{game.status}</span>
      </div>
    </div>
  );
}
