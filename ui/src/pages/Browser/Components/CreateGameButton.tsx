import { AnimatePresence, motion } from "motion/react";
import { useState } from "react";

export default function CreateGameButton() {
  const [roomName, setInput] = useState("");
  const [maxPlayers, setMaxPlayers] = useState(2);
  const [active, setActive] = useState(false);

  const handleClick = () => {
    if (!active) return setActive(true);
    if (roomName.trim().length === 0) return;
    if (maxPlayers < 2) return;

    window.bridges?.browser.callCreateGame(roomName, maxPlayers);
    setActive(false);
    setInput("");
    setMaxPlayers(2);
  };

  const handleRoomName = (e: React.ChangeEvent<HTMLInputElement>) => setInput(e.target.value);
  const handleMaxPlayers = (e: React.ChangeEvent<HTMLInputElement>) =>
    setMaxPlayers(Number(e.target.value));

  return (
    <div className="flex items-center gap-2">
      {active && (
        <>
          <input
            className="p-2 border-2"
            placeholder="Room name"
            type="text"
            value={roomName}
            onChange={handleRoomName}
          />

          <input
            className="p-2 border-2 w-14 text-right"
            type="number"
            value={maxPlayers}
            onChange={handleMaxPlayers}
            min={2}
          />
        </>
      )}

      <AnimatePresence initial={false}>
        <motion.button
          initial={{ width: "0px" }}
          animate={{ width: "auto" }}
          exit={{ width: "0px" }}
          layout
          className="p-2 px-8 justify-self-start border-2 active:bg-black active:text-white transition-colors bg-white"
          onClick={handleClick}
        >
          {active ? "Create" : "Create new game"}
        </motion.button>
      </AnimatePresence>
    </div>
  );
}
