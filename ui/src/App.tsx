import { useStore } from "zustand";
import LobbyPage from "./pages/Lobby";
import BrowserPage from "./pages/Browser";
import { mainStore } from "./store";
import GamePage from "./pages/Game";
import EndPage from "./pages/End";
import StartPage from "./pages/Start";
import { useMemo } from "react";
import "@/bridges"; // Inicjalizacja API

function App() {
  const view = useStore(mainStore, (state) => state.views.view);
  const setView = useStore(mainStore, (state) => state.views.setView);

  const Page = useMemo(() => {
    switch (view) {
      case "start":
        return <StartPage />;
      case "browser":
        return <BrowserPage />;
      case "room":
        return <LobbyPage />;
      case "game":
        return <GamePage />;
      case "end":
        return <EndPage />;

      default:
        return <div>Page not found</div>;
    }
  }, [view]);

  return (
    <div className="App">
      <div className="flex p-1 gap-2 border-b-2 mb-2 ">
        <h2>DevTools</h2>

        <button className="p-1 border-2" onClick={() => setView("start")}>
          Go to Start
        </button>
        <button className="p-1 border-2" onClick={() => setView("browser")}>
          Go to Browser
        </button>
        <button className="p-1 border-2" onClick={() => setView("room")}>
          Go to Lobby
        </button>
        <button className="p-1 border-2" onClick={() => setView("game")}>
          Go to Game
        </button>
        <button className="p-1 border-2" onClick={() => setView("end")}>
          Go to End
        </button>
      </div>

      {Page}
    </div>
  );
}

export default App;
