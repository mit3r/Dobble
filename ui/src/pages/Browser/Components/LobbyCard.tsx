import { GameStatusSpan } from "@/Components/Statuses";
import { mainStore } from "@/store";
import { GameStatus, Role, type ShortGameInfo } from "@/types/dobble";
import { useStore } from "zustand";

export default function LobbyCard({ game }: { game: ShortGameInfo }) {
  const isBlocked = useStore(mainStore, (state) => state.browser.isBlocked());

  const handlePlay = () => {
    window.bridges?.main.callMsg(
      `IP: ${game.ip}, Port: ${game.port}, GameID: ${game.gameId}, Role: Player`
    );

    window.bridges?.game.callJoinGame(game.ip, game.port, game.gameId, Role.Player);
  };

  const handleSpectate = () => {
    window.bridges?.game.callJoinGame(game.ip, game.port, game.gameId, Role.Observer);
  };

  const playersPercent = (game.players / game.maxPlayers) * 100;

  return (
    <div className="card flex flex-col gap-3 hover:border-dobble-primary">
      {/* Header */}
      <div className="flex items-start justify-between">
        <h2 className="font-bold text-lg text-dobble-text truncate flex-1">
          {game.gameName || "Unnamed Game"}
        </h2>
        <GameStatusSpan status={game.status} />
      </div>

      {/* Players Progress */}
      <div className="flex flex-col gap-1">
        <div className="flex justify-between text-sm">
          <span className="text-dobble-text-muted">Players</span>
          <span className="font-bold">
            {game.players}/{game.maxPlayers}
          </span>
        </div>
        <div className="h-2 bg-dobble-card rounded-full overflow-hidden">
          <div
            className="h-full bg-gradient-to-r from-dobble-primary to-dobble-secondary transition-all"
            style={{ width: `${playersPercent}%` }}
          />
        </div>
      </div>

      {/* Actions */}
      <div className="flex gap-2 mt-auto">
        {game.status === GameStatus.Waiting && (
          <button
            className="btn-primary flex-1 py-2 text-sm"
            onClick={handlePlay}
            disabled={isBlocked}
          >
            Play
          </button>
        )}

        {game.status !== GameStatus.Finished && (
          <button
            className="btn-secondary flex-1 py-2 text-sm"
            onClick={handleSpectate}
            disabled={isBlocked}
          >
            Watch
          </button>
        )}
      </div>
    </div>
  );
}
