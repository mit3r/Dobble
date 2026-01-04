import type { GameInfo, PlayerGameInfo } from "@/types/dobble";
import OtherPlayers from "./OtherPlayers";
import TopCardComponent from "./TopCardComponent";
import YourCardComponent from "./YourCardComponent";
import { mainStore } from "@/store";
import { useEffect } from "react";
import YourStats from "./YourStats";
import GamePlayFooter from "./GamePlayFooter";

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
      <h2 className="text-2xl w-full">{gameInfo.lastsCards} cards left</h2>
      <div className="flex-1 flex flex-col lg:flex-row overflow-hidden gap-4 justify-between ">
        <div className="h-full sm:max-h-100 flex-1 flex">
          <TopCardComponent disabled={you === null} cardId={gameInfo.topCardId} />
          {you !== null && <YourCardComponent yourInfo={you} />}
        </div>
        <div className="flex flex-col gap-2">
          {you !== null && <YourStats you={you} />}
          <OtherPlayers playersInfo={otherPlayers} observer={you === null} />
        </div>
      </div>

      <GamePlayFooter />
    </div>
  );
}
