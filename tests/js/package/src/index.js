import "../../../../out/build/release-emscripten/PREEMO.js";

let preemo;
let root;

//Component List
export const Components = Object.freeze({
  TransformComponent: Symbol("TransformComponent"),
  ScriptComponent: Symbol("ScriptComponent"),
});

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

  GetHandle() {
    return this.#handle;
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
        return new TransformComponent(this.#handle.AddComponentTransform());
      case Components.ScriptComponent:
        return new ScriptComponent(this.#handle.AddComponentScript(), this);
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

export class TransformComponent {
  #handle;
  constructor(handle) {
    this.#handle = handle;
  }
}

export class ScriptComponent {
  #handle;
  #entity;

  constructor(handle, entity) {
    this.#handle = handle;
    this.#entity = entity;
  }
  Bind(script) {
    const scriptInstance = new preemo.ScriptableEntityJS();
    scriptInstance.entity = this.#entity;
    scriptInstance.testNum = 1;
    // console.log(this.#entity.HasComponent(Components.TransformComponent));

    //On Create
    if (typeof script.prototype["OnCreate"] === "function") {
      scriptInstance.OnCreate = script.prototype["OnCreate"];
    } else {
      scriptInstance.OnCreate = function () {};
    }

    //On Update
    if (typeof script.prototype["OnUpdate"] === "function") {
      scriptInstance.OnUpdate = script.prototype["OnUpdate"];
    } else {
      scriptInstance.OnUpdate = function () {};
    }

    //On Destroy
    if (typeof script.prototype["OnDestroy"] === "function") {
      scriptInstance.OnDestroy = script.prototype["OnDestroy"];
    } else {
      scriptInstance.OnDestroy = function () {};
    }

    this.#handle.Bind(scriptInstance);
  }
}

export class Script {
  entity;
  HasComponent(type) {
    // this.entity.HasComponent(type);
    // console.log(this.testNum);
    // return this.testNum;
    return this.entity.HasComponent(type);
  }
  // testFunction() {
  //   // this.test = 2;
  //   console.log("yo " + this.test);
  // }
}

// export class Script extends preemo.ScriptableEntityJS {}

export function testScript() {
  console.log(preemo.ScriptableEntityJS);

  // const testEnt = new preemo.ScriptableEntityJS();
  // testEnt.name = name;
  // testEnt.OnCreate = function () {
  //   console.log("On Create");
  // };
  // testEnt.OnUpdate = function () {
  //   console.log("On Update ");
  // };
  // testEnt.OnDestroy = function () {
  //   console.log("On Destroy");
  // };

  // return testEnt;
}

export function Run(scene) {
  root.Run(scene.GetHandle());
}
