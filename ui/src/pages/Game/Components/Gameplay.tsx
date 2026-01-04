import type { GameInfo, PlayerGameInfo } from "@/types/dobble";
import OtherPlayers from "./OtherPlayers";
import TopCardComponent from "./TopCardComponent";
import YourCardComponent from "./YourCardComponent";
import { mainStore } from "@/store";
import { useEffect } from "react";
import YourStats from "./YourStats";
import PlayerFooter from "./PlayerFooter";
import ObserverFooter from "./ObserverFooter";

export default function Gameplay({
  gameInfo,
  you,
  otherPlayers,
}: {
  gameInfo: GameInfo;
  you: PlayerGameInfo | null;
  otherPlayers: PlayerGameInfo[];
}) {
  useEffect(() => {
    mainStore.getState().game.clearPicks();
  }, [gameInfo.turnId]);

  return (
    <div className="w-full h-full flex flex-col gap-4 flex-1">
      {/* Cards remaining indicator */}
      <div className="flex items-center justify-center gap-2">
        <span className="text-2xl font-bold text-dobble-text">
          {gameInfo.lastsCards} cards remaining
        </span>
      </div>

      {/* Main game area */}
      <div className="flex-1 flex flex-col lg:flex-row overflow-hidden gap-4 justify-between">
        {/* Cards section */}
        <div className="h-full sm:max-h-100 flex-1 flex gap-4">
          <TopCardComponent disabled={you === null} cardId={gameInfo.topCardId} />
          {you !== null && <YourCardComponent yourInfo={you} />}
        </div>

        {/* Stats & Players section */}
        <div className="flex flex-col gap-4 min-w-[280px]">
          {you !== null && <YourStats you={you} />}
          <OtherPlayers playersInfo={otherPlayers} observer={you === null} />
        </div>
      </div>

      {you !== null ? <PlayerFooter /> : <ObserverFooter />}
    </div>
  );
}
