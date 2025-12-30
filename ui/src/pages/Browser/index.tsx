import { mainStore } from "@/store";
import { useEffect, useMemo, useState } from "react";
import { useStore } from "zustand";
import LobbyCard from "./Components/LobbyCard";

export default function BrowserPage() {
  const games = useStore(mainStore, (state) => state.browser.games);
  const pageNumber = useStore(mainStore, (state) => state.browser.pageNumber);
  const nickname = useStore(mainStore, (state) => state.login.nickname);

  const prevPage = null;
  const nextPage = null;

  return (
    <div className="flex flex-col items-center gap-2 h-full relative">
      <div className="flex justify-center items-center w-full relative">
        <h1 className="text-4xl font-bold">Dobble</h1>
        <h2 className="absolute right-0">
          Hi, <span className="font-bold">{nickname ?? "Test object #1"}</span>
        </h2>
      </div>

      <div className="flex-1">
        {games === null ? (
          <LoadingGames />
        ) : (
          <div className="w-full p-2 grid grid-cols-2 grid-rows-2 gap-2">
            {games.map((game) => (
              <LobbyCard key={game.gameId} game={game} />
            ))}
          </div>
        )}
      </div>

      <div className="flex gap-4 w-full justify-center items-center relative">
        <button
          onClick={() => window.bridges?.browser.callNavigateToPage(0)}
          disabled={prevPage === null}
          className="p-2 border-2 disabled:opacity-50 disabled:cursor-not-allowed w-36"
        >
          Prev
        </button>

        <button
          onClick={() => window.bridges?.browser.callNavigateToPage(1)}
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
