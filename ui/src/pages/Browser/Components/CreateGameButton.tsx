import { AnimatePresence, motion } from "motion/react";
import { useState } from "react";

export default function CreateGameButton() {
  const [roomName, setInput] = useState("");
  // const [maxPlayers, setMaxPlayers] = useState(2);
  const [active, setActive] = useState(false);

  const handleClick = () => {
    if (!active) return setActive(true);
    if (roomName.trim().length === 0) return;
    // if (maxPlayers < 2) return;

    window.bridges?.browser.callCreateGame(roomName, 4);
    setActive(false);
    setInput("");
    // setMaxPlayers(2);
  };

  const handleCancel = () => {
    setActive(false);
    setInput("");
  };

  const handleRoomName = (e: React.ChangeEvent<HTMLInputElement>) => setInput(e.target.value);
  // const handleMaxPlayers = (e: React.ChangeEvent<HTMLInputElement>) =>
  //   setMaxPlayers(Number(e.target.value));

  return (
    <div className="flex items-center gap-3">
      <AnimatePresence>
        {active && (
          <motion.div
            initial={{ opacity: 0, x: -20 }}
            animate={{ opacity: 1, x: 0 }}
            exit={{ opacity: 0, x: -20 }}
            className="flex gap-2"
          >
            <input
              className="input-field"
              placeholder="Room name..."
              type="text"
              value={roomName}
              onChange={handleRoomName}
              autoFocus
            />
            <button onClick={handleCancel} className="btn-danger py-2 px-4">
              X
            </button>
          </motion.div>
        )}
      </AnimatePresence>

      <motion.button
        layout
        className="btn-success"
        onClick={handleClick}
      >
        {active ? "Create" : "New Game"}
      </motion.button>
    </div>
  );
}
