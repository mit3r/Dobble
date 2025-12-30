import { useMemo } from "react";
import { useStore } from "zustand";
import BrowserPage from "./pages/Browser";
import EndPage from "./pages/End";
import GamePage from "./pages/Game";
import LobbyPage from "./pages/Lobby";
import LoginPage from "./pages/Login";
import { mainStore } from "./store";
import { View } from "./types/dobble";

function App() {
  const view = useStore(mainStore, (state) => state.main.view);
  const setView = useStore(mainStore, (state) => state.main.setView);

  const Page = useMemo(() => {
    switch (view) {
      case View.Login:
        return <LoginPage />;
      case View.Browser:
        return <BrowserPage />;
      case View.Room:
        return <LobbyPage />;
      case View.Game:
        return <GamePage />;
      case View.End:
        return <EndPage />;

      default:
        return <div>Page not found</div>;
    }
  }, [view]);

  return (
    <div className="flex flex-col h-screen select-none">
      <div className="h-14 flex p-1 gap-2 border-2 min-h-14 ">
        <h2>DevTools</h2>

        <button className="p-1 border-2" onClick={() => setView(View.Login)}>
          Go to Start
        </button>
        <button className="p-1 border-2" onClick={() => setView(View.Browser)}>
          Go to Browser
        </button>
        <button className="p-1 border-2" onClick={() => setView(View.Room)}>
          Go to Lobby
        </button>
        <button className="p-1 border-2" onClick={() => setView(View.Game)}>
          Go to Game
        </button>
        <button className="p-1 border-2" onClick={() => setView(View.End)}>
          Go to End
        </button>
      </div>

      <div className="flex-1 overflow-hidden p-4">{Page}</div>
    </div>
  );
}

export default App;
