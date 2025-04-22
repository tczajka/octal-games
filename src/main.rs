use clap::{Parser, Subcommand};

#[derive(Clone, Debug, Parser)]
#[command(about, long_about = None)]
struct CommandLine {
    #[command(subcommand)]
    command: Command,
}

#[derive(Clone, Debug, Subcommand)]
enum Command {
    Brute,
}

fn main() {
    let command_line = CommandLine::parse();
}