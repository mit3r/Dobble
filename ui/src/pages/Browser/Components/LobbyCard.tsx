import { GameStatusSpan } from "@/Components/Statuses";
import { mainStore } from "@/store";
import type { ShortGameInfo } from "@/types/dobble";
import { useStore } from "zustand";

export default function LobbyCard({ game }: { game: ShortGameInfo }) {
  const isBlocked = useStore(mainStore, (state) => state.browser.isBlocked());

  const handlePlay = () => {
    window.bridges?.browser.callJoinGame(game.gameId);
  };

  const handleSpectate = () => {
    window.bridges?.browser.callObserveGame(game.gameId);
  };

  return (
    <div className="grid grid-cols-1 grid-rows-3 border-2 p-2">
      <h2>{game.gameName}</h2>

      <span>
        Players: {game.players}/{game.maxPlayers}
      </span>

      <div className="flex gap-2 justify-start text-center items-baseline">
        <button
          className="basis-0 p-1 flex-1 border-2 disabled:opacity-50"
          onClick={handlePlay}
          disabled={isBlocked}
        >
          Play
        </button>
        <button
          className="basis-0 p-1 flex-1 border-2 disabled:opacity-50"
          onClick={handleSpectate}
          disabled={isBlocked}
        >
          Spectate
        </button>

        <span className="text-right flex-2">
          <GameStatusSpan status={game.status} />
        </span>
      </div>
    </div>
  );
}
