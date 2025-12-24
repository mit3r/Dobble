import { useStore } from "zustand";
import LobbyPage from "./pages/Lobby";
import StartPage from "./pages/Start";
import { mainStore } from "./store";
import useQWebChannel from "./hooks/useQWebChannel";

function App() {
  useQWebChannel();

  const page = useStore(mainStore, (state) => state.page);

  switch (page) {
    case "start":
      return <StartPage />;
    case "room":
      return <LobbyPage />;

    default:
      return <div>Page not found</div>;
  }
}

export default App;
