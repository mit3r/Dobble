import { getCardSymbols } from "@/utils/CardsSymbols";
import { useMemo } from "react";
import IconComponent from "./IconComponent";

export default function CardComponent(props: { className?: string; cardId: number }) {
  const [first, ...icons] = useMemo(() => getCardSymbols(props.cardId), [props.cardId]);

  return (
    <div className="aspect-square relative h-full rounded-full border-4 overflow-clip">
      <IconComponent
        cardId={props.cardId}
        iconId={first}
        minSize={15}
        maxSize={33}
        left={50}
        top={50}
      />

      {icons.map((iconId, index) => {
        const angle = (index / icons.length + props.cardId + props.cardId) * 2 * Math.PI;
        const radius = 33; // 33% from center
        const left = 50 + radius * Math.cos(angle);
        const top = 50 + radius * Math.sin(angle);

        return (
          <IconComponent
            key={index}
            cardId={props.cardId}
            iconId={iconId}
            minSize={15}
            maxSize={33}
            left={left}
            top={top}
          />
        );
      })}
    </div>
  );
}
