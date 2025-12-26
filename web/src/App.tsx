import { useStore } from "zustand";
import LobbyPage from "./pages/Lobby";
import StartPage from "./pages/Start";
import { mainStore } from "./store";
import useQWebChannel from "./hooks/useQWebChannel";
import GamePage from "./pages/Game";
import EndPage from "./pages/End";
import { useMemo } from "react";

function App() {
  useQWebChannel();

  const page = useStore(mainStore, (state) => state.page);
  const setPage = useStore(mainStore, (state) => state.setPage);

  const Page = useMemo(() => {
    switch (page) {
      case "start":
        return <StartPage />;
      case "room":
        return <LobbyPage />;
      case "game":
        return <GamePage />;
      case "end":
        return <EndPage />;

      default:
        return <div>Page not found</div>;
    }
  }, [page]);

  return (
    <div className="App">
      <div className="flex p-1 gap-2 border-b-2 mb-2 ">
        <h2>DevTools</h2>

        <button className="p-1 border-2" onClick={() => setPage("start")}>
          Go to Start
        </button>
        <button className="p-1 border-2" onClick={() => setPage("room")}>
          Go to Lobby
        </button>
        <button className="p-1 border-2" onClick={() => setPage("game")}>
          Go to Game
        </button>
        <button className="p-1 border-2" onClick={() => setPage("end")}>
          Go to End
        </button>
      </div>

      {Page}
    </div>
  );
}

export default App;
