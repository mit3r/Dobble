import GameServStatus from "@/Components/ServerStatus/GameServStatus";
import LobbyServStatus from "@/Components/ServerStatus/LobbyServStatus";
import { mainStore } from "@/store";
import { CommunicationStatus, ConnectionStatus } from "@/types/dobble";
import { useEffect, useMemo, useRef, useState } from "react";
import { useStore } from "zustand";
import CreateGameButton from "./Components/CreateGameButton";
import LobbyCard from "./Components/LobbyCard";

export default function BrowserPage() {
  const games = useStore(mainStore, (state) => state.browser.games);
  const pageNumber = useStore(mainStore, (state) => state.browser.currentPageNumber);
  const nickname = useStore(mainStore, (state) => state.main.nickname);
  
  const lobbyCommunication = useStore(mainStore, (state) => state.browser.lobbyCommunication);
  const lobbyConnection = useStore(mainStore, (state) => state.browser.lobbyConnection);
  
  const isBlocked = lobbyCommunication === CommunicationStatus.Failed || 
                    lobbyCommunication === CommunicationStatus.Retrying ||
                    lobbyConnection !== ConnectionStatus.Connected;

  const nextPage = useStore(mainStore, (state) => state.browser.nextPageNumber);
  const prevPage = useMemo(() => {
    if (pageNumber === null) return null;
    return pageNumber > 1 ? pageNumber - 1 : null;
  }, [pageNumber]);

  const pageNumberRef = useRef(pageNumber);
  pageNumberRef.current = pageNumber;

  useEffect(() => {
    const it = setInterval(() => {
      window.bridges?.browser.callNavigateToPage(pageNumberRef.current ?? 1);
    }, 1_000);

    return () => clearInterval(it);
  }, []); // Empty dependency array - interval is created only once

  return (
    <div className="flex flex-col gap-6 h-full p-2">
      {/* Status Bar */}
      <div className="flex gap-4 justify-end">
        <LobbyServStatus />
        <GameServStatus />
      </div>

      {/* Header */}
      <div className="flex justify-between items-center">
        <CreateGameButton />
        <h1 className="page-title text-3xl">Dobble</h1>
        <div className="text-right">
          <span className="text-dobble-text-muted">Playing as</span>
          <h2 className="font-bold text-xl text-dobble-primary">{nickname ?? "Guest"}</h2>
        </div>
      </div>

      {/* Games Grid */}
      <div className="flex-1 overflow-y-auto custom-scrollbar">
        {games === null ? (
          <LoadingGames />
        ) : games.length === 0 ? (
          <div className="panel text-center py-16">
            <p className="text-dobble-text-muted">No games available</p>
            <p className="text-sm text-dobble-text-muted mt-2">Create a new game to get started!</p>
          </div>
        ) : (
          <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4 gap-4">
            {games.map((game) => (
              <LobbyCard key={game.gameId} game={game} isBlocked={isBlocked} />
            ))}
          </div>
        )}
      </div>

      {/* Pagination */}
      <div className="flex gap-4 justify-center items-center">
        <button
          onClick={() => window.bridges?.browser.callNavigateToPage(prevPage ?? 1)}
          disabled={prevPage === null}
          className="btn-secondary"
        >
          ← Previous
        </button>

        {pageNumber && (
          <span className="px-4 py-2 text-dobble-text-muted">
            Page <span className="font-bold text-dobble-text">{pageNumber}</span>
          </span>
        )}

        <button
          onClick={() => window.bridges?.browser.callNavigateToPage(nextPage ?? 1)}
          disabled={nextPage === null}
          className="btn-secondary"
        >
          Next →
        </button>
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

  return (
    <div className="panel text-center py-16">
      <div className="animate-pulse">
        <p className="text-xl text-dobble-text-muted">Loading games{dots}</p>
      </div>
    </div>
  );
}
