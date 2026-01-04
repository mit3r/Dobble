import { useMemo } from "react";
import { useStore } from "zustand";
import BrowserPage from "./pages/Browser";
import EndPage from "./pages/End";
import GamePage from "./pages/Game";
import LobbyPage from "./pages/Lobby";
import LoginPage from "./pages/Login";
import { mainStore } from "./store";
import { View } from "./types/dobble";
// import DevTools from "./Components/DevTools";

function App() {
  const view = useStore(mainStore, (state) => state.main.view);

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
    <div className="flex flex-col h-screen select-none min-w-lg">
      {/* <DevTools /> */}
      <div className="flex-1 overflow-hidden p-4">{Page}</div>
    </div>
  );
}

export default App;
