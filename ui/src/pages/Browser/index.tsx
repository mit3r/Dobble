import { GameStatus } from "@/types/dobble";
import LobbyCard from "./Components/LobbyCard";

const lobbyInfo = {
  actualGames: [
    {
      gameId: "1",
      gameName: "Fun Game",
      players: 3,
      maxPlayers: 6,
      status: GameStatus.Finished,
      nicknames: ["Alice", "Bob", "Charlie"],
    },
    {
      gameId: "2",
      gameName: "Serious Match",
      players: 5,
      maxPlayers: 5,
      status: GameStatus.InGame,
      nicknames: ["Dave", "Eve", "Frank", "Grace", "Heidi"],
    },
  ],
  nextPage: 2,
  prevPage: null,
  page: 1,
};

export default function BrowserPage() {
  // const setPage = useStore(mainStore, (state) => state.setPage);

  return (
    <div className="flex flex-col items-center gap-2">
      <h1 className="text-4xl font-bold mb-4">Dobble</h1>

      <ol className="w-full p-2 grid grid-cols-2 grid-rows-2 gap-2">
        {lobbyInfo.actualGames.map((game) => (
          <li key={game.gameId}>
            <LobbyCard game={game} />
          </li>
        ))}
      </ol>

      <div className="flex gap-4 w-full justify-center items-center relative">
        <button
          onClick={() => window.bridges?.browser.navigateToPage(0)}
          disabled={lobbyInfo.prevPage === null}
          className="p-2 border-2 disabled:opacity-50 disabled:cursor-not-allowed w-36"
        >
          Prev
        </button>

        <button
          onClick={() => window.bridges?.browser.navigateToPage(1)}
          disabled={lobbyInfo.nextPage === null}
          className="p-2 border-2 disabled:opacity-50 disabled:cursor-not-allowed w-36"
        >
          Next
        </button>

        <span className="absolute right-0 p-2 px-4">Page {lobbyInfo.page}</span>
      </div>
    </div>
  );
}
