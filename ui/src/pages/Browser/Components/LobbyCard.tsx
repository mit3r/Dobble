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

  return (
    <div className="grid grid-cols-2 grid-rows-3 border-2 p-2 gap-2">
      <h2 className="col-span-2 font-bold">{game.gameName}</h2>

      <span className="col-span-1">
        Players: {game.players}/{game.maxPlayers}
      </span>

      <span className="col-span-1 text-right ">
        <GameStatusSpan status={game.status} />
      </span>

      <div className="flex gap-2 col-span-2">
        {game.status === GameStatus.Waiting && (
          <button
            className="flex-1 p-1  border-2 disabled:opacity-50"
            onClick={handlePlay}
            disabled={isBlocked}
          >
            Play
          </button>
        )}

        {game.status !== GameStatus.Finished && (
          <button
            className="flex-1 p-1  border-2 disabled:opacity-50"
            onClick={handleSpectate}
            disabled={isBlocked}
          >
            Spectate
          </button>
        )}
      </div>
    </div>
  );
}
