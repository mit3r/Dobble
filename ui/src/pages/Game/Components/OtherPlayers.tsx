import type { PlayerGameInfo } from "@/types/dobble";
import { clsx } from "clsx";
import CardComponent from "./CardComponent";

export default function OtherPlayers(props: { observer: boolean; playersInfo: PlayerGameInfo[] }) {
  return (
    <div className=" flex flex-col overflow-hidden h-full">
      <h2 className="p-2 border-b-2 w-full">Other players</h2>

      <div className="flex-1 grid grid-cols-1 2xl:grid-cols-2 min-h-32 auto-rows-min overflow-y-auto overflow-x-hidden">
        {props.playersInfo.map((player) => (
          <OtherPlayerComponent key={player.nickname} player={player} extended={props.observer} />
        ))}
      </div>
    </div>
  );
}

function OtherPlayerComponent(props: { player: PlayerGameInfo; extended?: boolean }) {
  return (
    <div
      className={clsx(
        "overflow-clip flex justify-between gap-2 p-2 px-4 nth-[2n+1]:bg-gray-200 ",
        props.extended ? "h-40" : "h-32"
      )}
    >
      <div className="flex flex-col h-full shrink-0 ">
        <h3 className="text-xl font-bold">{props.player.nickname}</h3>
        <p>Points: {props.player.points}</p>
        <p>Matches: {props.player.matches}</p>
        <p>Mistakes: {props.player.mistakes}</p>
      </div>

      <div className="h-full flex justify-end">
        <CardComponent cardId={props.player.cardId} disabled={true} />
      </div>
    </div>
  );
}
