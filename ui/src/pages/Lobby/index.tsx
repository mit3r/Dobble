import { useStore } from "zustand";
import { mainStore } from "../../store";

export default function LobbyPage() {
  const lobbyInfo = useStore(mainStore, (state) => state.lobby.info);

  return (
    <div>
      <h1>Lobby Page</h1>
      <pre>{JSON.stringify(lobbyInfo, null, 2)}</pre>
    </div>
  );
}
