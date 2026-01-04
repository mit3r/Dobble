import Loading from "@/Components/Loading";
import LobbyBar from "@/Components/LobbyBar";
import QuitFooter from "@/Components/QuitFooter";
import { mainStore } from "@/store";
import { useMemo } from "react";
import { useStore } from "zustand";
import Gameplay from "./Components/Gameplay";

export default function GamePage() {
  const gameInfo = useStore(mainStore, (state) => state.game.gameInfo);

  const nickname = useStore(mainStore, (state) => state.main.nickname);

  const you = useMemo(() => {
    return gameInfo?.players.find((player) => player.nickname === nickname) || null;
  }, [gameInfo, nickname]);

  const otherPlayers = useMemo(() => {
    return gameInfo?.players.filter((player) => player.nickname !== nickname);
  }, [gameInfo, nickname]);

  return (
    <div className="w-full h-full flex  flex-col gap-2 ">
      <LobbyBar />

      {!gameInfo || !otherPlayers ? (
        <>
          <div className="flex-1 flex items-center justify-center">
            <Loading />
          </div>
          <QuitFooter onQuit={() => window.bridges?.game.callQuitGame()} />
        </>
      ) : (
        <Gameplay gameInfo={gameInfo} you={you} otherPlayers={otherPlayers} />
      )}
    </div>
  );
}
