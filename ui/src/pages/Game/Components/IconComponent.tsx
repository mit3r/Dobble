import { getIconsSource, quasiRandomRotation } from "@/utils/CardsSymbols";
import { useMemo } from "react";
import clsx from "clsx";
import { twMerge } from "tailwind-merge";

export default function IconComponent(props: {
  cardId: number;
  iconId: number;
  minSize: number;
  maxSize: number;
  left: number;
  top: number;
  picked?: boolean;
  onClick?: () => void;
}) {
  const src = useMemo(() => getIconsSource(props.iconId), [props.iconId]);
  const rotation = useMemo(
    () => quasiRandomRotation(props.cardId, props.iconId),
    [props.cardId, props.iconId]
  );

  const size = useMemo(() => {
    const sizeRange = props.maxSize - props.minSize;
    const pseudoRandomFactor = ((props.cardId + props.iconId) * 37) % 100;
    return props.minSize + (pseudoRandomFactor / 100) * sizeRange;
  }, [props.cardId, props.iconId, props.minSize, props.maxSize]);

  return (
    <div
      className="absolute aspect-square  transition-transform"
      style={{
        width: `${size}%`,
        height: `${size}%`,
        left: `${props.left}%`,
        top: `${props.top}%`,
        transform: `translate(-50%, -50%) rotate(${rotation}deg)`,
      }}
    >
      <img
        onClick={props.onClick}
        draggable={false}
        className={twMerge(
          clsx(
            " w-full h-full object-contain hover:scale-110 active:scale-95 transition-all",
            props.picked && "drop-shadow-[0_0_10px_red]"
          )
        )}
        src={src}
        alt={`Icon ${props.iconId}`}
      />
    </div>
  );
}
