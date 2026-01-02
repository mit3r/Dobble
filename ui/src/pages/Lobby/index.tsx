import LobbyBar from "@/Components/LobbyBar";
import QuitFooter from "@/Components/QuitFooter";
import { mainStore } from "@/store";
import { useStore } from "zustand/react";
import LoadingLobby from "./Components/LoadingLobby";
import PlayersList from "./Components/PlayersList";

export default function LobbyPage() {
  const gameInfo = useStore(mainStore, (state) => state.game.gameInfo);

  return (
    <div className="h-full flex flex-col justify-end gap-4">
      <LobbyBar />

      {gameInfo === null ? <LoadingLobby /> : <PlayersList players={gameInfo.players} />}

      <QuitFooter onQuit={() => {}} />
    </div>
  );
}
