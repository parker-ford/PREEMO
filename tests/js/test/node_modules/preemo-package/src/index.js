import "../../../../out/build/release-emscripten/PREEMO.js";

//Component List
export const Components = Object.freeze({
  TransformComponent: Symbol("TransformComponent"),
});

let preemo;
let root;

export async function initialize() {
  preemo = await loadPreemo();
  root = new preemo.Root();
  await startUpRoot();
}

function startUpRoot() {
  return new Promise((resolve, reject) => {
    if (!root) {
      reject(new Error("Root not initialized"));
      return;
    }

    root.StartUp(null);

    // Hacky way to wait for root startup complete. I could not figure out how to create a real async funciton with emscriptenn
    function checkStartUpComplete() {
      if (root.hasCompletedStartUp()) {
        resolve();
      } else {
        setTimeout(checkStartUpComplete, 100); // Poll every 100ms
      }
    }

    // Start checking
    checkStartUpComplete();

    const timeout = 30000; // 30 seconds timeout
    setTimeout(() => {
      reject(new Error("Preemo root StartUp timed out"));
    }, timeout);
  });
}

function loadPreemo() {
  return new Promise((resolve, reject) => {
    const scriptTag = document.createElement("script");
    scriptTag.src = "../../../../out/build/release-emscripten/PREEMO.js";

    scriptTag.onload = () => {
      console.log("PREEMO.js loaded successfully");
      if (typeof window.createModule === "function") {
        window
          .createModule()
          .then(async function (Module) {
            resolve(Module);
          })
          .catch(reject);
      } else {
        reject(new Error("createModule is not a function"));
      }
    };

    scriptTag.onerror = () => {
      reject(new Error("Failed to load PREEMO.js"));
    };

    document.body.appendChild(scriptTag);
  });
}

function checkInitialized() {
  if (!preemo) {
    console.log("Preemo not initialized. Call PREEMO.intialize() first!");
    return false;
  }
  return true;
}

export class Scene {
  #handle;

  constructor() {
    console.log("Create Scene");
    if (!checkInitialized()) {
      return;
    }
    this.#handle = new preemo.Scene();
  }

  CreateEntity() {
    const entHandle = this.#handle.CreateEntity();
    return new Entity(entHandle);
  }
}

export class Entity {
  #handle;
  constructor(handle) {
    this.#handle = handle;
  }

  AddComponent(type) {
    switch (type) {
      case Components.TransformComponent:
        this.#handle.AddComponentTransform();
        break;
      default:
        console.log("default case");
    }
  }

  HasComponent(type) {
    switch (type) {
      case Components.TransformComponent:
        return this.#handle.HasComponentTransform();
      default:
        console.log("default case");
    }
  }
}
