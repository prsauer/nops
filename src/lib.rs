#![deny(clippy::all)]
use napi_derive::napi;
use std::time::SystemTime;
use sysinfo::{ProcessRefreshKind, RefreshKind, System};

#[napi]
pub fn plus_100(input: u32) -> u32 {
  input + 100
}

#[napi]
pub fn plus_200(input: u32) -> u32 {
  input + 200
}

#[napi]
pub fn get_system_info() -> Vec<String> {
  let mut system = System::new_all();
  let start = SystemTime::now();

  system.refresh_specifics(
    RefreshKind::nothing().with_processes(ProcessRefreshKind::everything().without_cpu()),
  );
  system.processes();

  let duration = SystemTime::now().duration_since(start).unwrap();
  println!("it took {}ms", duration.as_millis());

  let names = system
    .processes()
    .iter()
    .map(|(_, process)| process.name().to_string_lossy().to_string())
    .collect();

  return names;
}
