import { GameStatusSpan } from "@/Components/Statuses";
import { mainStore } from "@/store";
import { useStore } from "zustand/react";
import Loading from "../Loading";

export default function LobbyBar() {
  const gameInfo = useStore(mainStore, (state) => state.game.gameInfo);

  return (
    <div className="flex items-center justify-center relative h-fit py-4 w-full border-y-2">
      <h2 className="flex-1 basis-0 text-xl text-left ">
        Room {!gameInfo ? <Loading /> : gameInfo.gameName}
      </h2>
      <h1 className="flex-1 basis-0 text-4xl font-bold text-center">Dobble</h1>
      <h2 className="flex-1 basis-0 text-xl text-right">
        {!gameInfo ? <Loading /> : <GameStatusSpan status={gameInfo.status} />}
      </h2>
    </div>
  );
}
