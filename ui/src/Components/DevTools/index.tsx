import { mainStore } from "@/store";
import { View } from "@/types/dobble";
import { useStore } from "zustand/react";

export default function DevTools() {
  const setView = useStore(mainStore, (state) => state.main.setView);

  return (
    <div className="h-14 flex p-1 gap-2 border-2 min-h-14 ">
      <h2>DevTools</h2>

      <button className="p-1 border-2" onClick={() => setView(View.Login)}>
        Go to Start
      </button>
      <button className="p-1 border-2" onClick={() => setView(View.Browser)}>
        Go to Browser
      </button>
      <button className="p-1 border-2" onClick={() => setView(View.Room)}>
        Go to Lobby
      </button>
      <button className="p-1 border-2" onClick={() => setView(View.Game)}>
        Go to Game
      </button>
      <button className="p-1 border-2" onClick={() => setView(View.End)}>
        Go to End
      </button>
    </div>
  );
}
