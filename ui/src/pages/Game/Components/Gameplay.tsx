import { mainStore } from "@/store";
import type { GameInfo, PlayerGameInfo } from "@/types/dobble";
import { useEffect } from "react";
import OtherPlayers from "./OtherPlayers";
import TopCardComponent from "./TopCardComponent";
import YourCardComponent from "./YourCardComponent";

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
    return () => mainStore.getState().game.clearPicks();
  }, [otherPlayers]);

  return (
    <div className="w-full h-full flex flex-col gap-4 flex-1">
      <h2 className="text-2xl w-full">{gameInfo.lastsCards} cards left</h2>
      <div className="flex-1 flex flex-col lg:flex-row overflow-hidden gap-4 justify-between ">
        <div className="h-full sm:max-h-100 flex-1 flex">
          <TopCardComponent disabled={you === null} cardId={gameInfo.topCardId} />
          {you !== null && <YourCardComponent yourInfo={you} />}
        </div>
        <OtherPlayers playersInfo={otherPlayers} observer={you === null} />
      </div>
    </div>
  );
}
