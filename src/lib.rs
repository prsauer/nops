#![deny(clippy::all)]
use napi_derive::napi;
use std::time::SystemTime;
use sysinfo::{ProcessRefreshKind, RefreshKind, System};

#[napi]
pub fn get_system_info() -> Vec<String> {
  let start = SystemTime::now();
  let system = System::new_with_specifics(
    RefreshKind::nothing()
      .with_processes(ProcessRefreshKind::nothing().with_exe(sysinfo::UpdateKind::Always)),
  );

  let names = system
    .processes()
    .iter()
    .map(|(_, process)| process.name().to_string_lossy().to_string())
    .collect();

  let duration = SystemTime::now().duration_since(start).unwrap();
  println!("it took {}ms", duration.as_millis());

  return names;
}
