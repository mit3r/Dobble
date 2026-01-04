import { mainStore } from "@/store";
import type { PlayerGameInfo } from "@/types/dobble";
import { useStore } from "zustand";
import CardComponent from "./CardComponent";

export default function YourCardComponent(props: { yourInfo: PlayerGameInfo }) {
  const handPick = useStore(mainStore, (state) => state.game.handPick);

  const pickHandCard = useStore(mainStore, (state) => state.game.pickHandCard);

  return (
    <div className="flex-1 flex flex-col items-center gap-4 justify-center">
      <CardComponent
        cardId={props.yourInfo.cardId}
        pickedIconId={handPick}
        onIconClick={pickHandCard}
      />

      <h2 className="text-center text-xl font-bold text-dobble-primary">
        Your card
      </h2>
    </div>
  );
}
