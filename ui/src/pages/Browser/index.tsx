import { mainStore } from "@/store";
import { useEffect, useMemo, useState } from "react";
import { useStore } from "zustand";
import LobbyCard from "./Components/LobbyCard";
import CreateGameButton from "./Components/CreateGameButton";
import LobbyServerStatus from "@/Components/LobbyServerStatus";

export default function BrowserPage() {
  const games = useStore(mainStore, (state) => state.browser.games);
  const pageNumber = useStore(mainStore, (state) => state.browser.currentPageNumber);
  const nickname = useStore(mainStore, (state) => state.main.nickname);

  const nextPage = useStore(mainStore, (state) => state.browser.nextPageNumber);
  const prevPage = useMemo(() => {
    if (pageNumber === null) return null;
    return pageNumber > 1 ? pageNumber - 1 : null;
  }, [pageNumber]);

  useEffect(() => {
    const it = setInterval(() => {
      window.bridges?.browser.callNavigateToPage(pageNumber ?? 1);
    }, 1_000);

    return () => clearInterval(it);
  }, []);

  return (
    <div className="flex flex-col items-center gap-8 h-full  p-2">
      <LobbyServerStatus />

      <div className="grid grid-cols-3 w-full items-center">
        <CreateGameButton />

        <h1 className="text-center text-4xl font-bold">Dobble</h1>
        <h2 className="text-right ">
          Hi, <span className="font-bold">{nickname ?? "Test object #1"}</span>
        </h2>
      </div>

      <div className="flex-1 w-full">
        {games === null ? (
          <LoadingGames />
        ) : games.length === 0 ? (
          <div className="p-20">No games available</div>
        ) : (
          <div className="w-full grid grid-cols-4 grid-rows-3 gap-2">
            {games.map((game) => (
              <LobbyCard key={game.gameId} game={game} />
            ))}
          </div>
        )}
      </div>

      <div className="flex gap-4 w-full justify-center items-center relative">
        <button
          onClick={() => window.bridges?.browser.callNavigateToPage(prevPage ?? 1)}
          disabled={prevPage === null}
          className="p-2 border-2 disabled:opacity-50 disabled:cursor-not-allowed w-36"
        >
          Prev
        </button>

        <button
          onClick={() => window.bridges?.browser.callNavigateToPage(nextPage ?? 1)}
          disabled={nextPage === null}
          className="p-2 border-2 disabled:opacity-50 disabled:cursor-not-allowed w-36"
        >
          Next
        </button>

        {pageNumber && <span className="absolute right-0 p-2 px-4">Page {pageNumber}</span>}
      </div>
    </div>
  );
}

function LoadingGames() {
  const [stage, setStage] = useState(0);
  const dots = useMemo(() => ".".repeat(stage), [stage]);

  const handleProgress = () => setStage((prev) => (prev + 1) % 4);

  useEffect(() => {
    const interval = setInterval(handleProgress, 500);
    return () => clearInterval(interval);
  }, []);

  return <div className="p-20">Loading games{dots}</div>;
}
