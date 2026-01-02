import { getCardSymbols } from "@/utils/CardsSymbols";
import { useMemo } from "react";
import IconComponent from "./IconComponent";
import clsx from "clsx";

export default function CardComponent(props: {
  className?: string;
  cardId: number;
  pickedIconPosition?: number;
  onIconClick?: (iconPosition: number) => void;
  disabled?: boolean;
}) {
  const [first, ...icons] = useMemo(() => getCardSymbols(props.cardId), [props.cardId]);

  return (
    <div
      className={clsx(
        "aspect-square  flex-1 relative rounded-full border-4 overflow-clip bg-white",
        props.disabled ? "pointer-events-none" : "pointer-events-auto"
      )}
    >
      <IconComponent
        picked={props.pickedIconPosition === 0}
        onClick={() => !props.disabled && props.onIconClick?.(0)}
        cardId={props.cardId}
        iconId={first}
        minSize={15}
        maxSize={33}
        left={50}
        top={50}
      />
      {icons.map((iconId, i) => {
        const index = i + 1;
        const angle = (index / icons.length + props.cardId + props.cardId) * 2 * Math.PI;
        const radius = 33; // 33% from center
        const left = 50 + radius * Math.cos(angle);
        const top = 50 + radius * Math.sin(angle);
        return (
          <IconComponent
            picked={props.pickedIconPosition === index}
            onClick={() => !props.disabled && props.onIconClick?.(index)}
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
