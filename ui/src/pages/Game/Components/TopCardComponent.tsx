import { mainStore } from "@/store";
import { useStore } from "zustand/react";
import CardComponent from "./CardComponent";

export default function TopCardComponent(props: { cardId: number; disabled?: boolean }) {
  const topPick = useStore(mainStore, (state) => state.game.topPick);

  const pickTopCard = useStore(mainStore, (state) => state.game.pickTopCard);

  return (
    <div className="flex-1 h-full flex flex-col items-center justify-center gap-2 ">
      <CardComponent
        cardId={props.cardId}
        pickedIconPosition={topPick}
        onIconClick={pickTopCard}
        disabled={props.disabled}
      />
      <h2 className=" text-center text-2xl">Card on the top</h2>
    </div>
  );
}
