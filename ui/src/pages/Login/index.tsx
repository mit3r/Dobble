import LobbyServStatus from "@/Components/ServerStatus/LobbyServStatus";
import { mainStore } from "@/store";
import { useStore } from "zustand";
import NicknameComponent from "./Components/NicknameComponent";
import AddressComponent from "./Components/AddressComponent";

export default function LoginPage() {
  const lobbyAddress = useStore(mainStore, (s) => s.browser.lobbyAddress);

  return (
    <div className="h-full flex flex-col items-center justify-center">
      {lobbyAddress && (
        <div className="absolute top-4 right-4">
          <LobbyServStatus />
        </div>
      )}

      <div className="panel flex flex-col gap-8 max-w-md w-full">
        <div className="text-center">
          <h1 className="page-title mb-4">Dobble</h1>
          <p className="subtitle">Welcome to the Dobble card game!</p>
        </div>
        
        <div className="flex-1">
          {lobbyAddress ? <NicknameComponent /> : <AddressComponent />}
        </div>
      </div>
    </div>
  );
}
