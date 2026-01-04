import { AnimatePresence } from "motion/react";
import AlertGoodMatch from "./Alerts/AlertGoodMatch";
import AlertMistake from "./Alerts/AlertMistake";
import AlertTooSlow from "./Alerts/AlertTooSlow";
import { useEffect } from "react";
import { useStore } from "zustand";
import { mainStore } from "@/store";
import { MatchResult } from "@/types/dobble";

export default function PlayerFooter() {
  const matchResult = useStore(mainStore, (s) => s.game.matchResult);

  const clearMatchResult = useStore(mainStore, (s) => s.game.clearMatchResult);

  useEffect(() => {
    const to = setTimeout(clearMatchResult, 2000);
    return () => clearTimeout(to);
  }, [matchResult]);

  return (
    <div className="w-full grid grid-cols-[1fr_auto] gap-4 border-t-2 p-4">
      <div className="flex justify-center items-center">
        <AnimatePresence>
          {matchResult === MatchResult.Correct && <AlertGoodMatch />}
          {matchResult === MatchResult.Incorrect && <AlertMistake />}
          {matchResult === MatchResult.TooLate && <AlertTooSlow />}
        </AnimatePresence>
      </div>

      <button className="p-2 px-4 border-2" onClick={() => window.bridges?.game.callQuitGame()}>
        Quit Game
      </button>
    </div>
  );
}
