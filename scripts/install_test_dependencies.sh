#!/bin/bash

set -e

echo "Installing test dependencies..."

echo "1/2 Installing Ceedling..."
gem install ceedling -v 1.0.1 --user-install

echo "2/2 Installing gcovr..."
pip3 install --user gcovr

echo ""
echo "✅ Installation complete!"
echo ""
echo "If commands not found, add to PATH:"
echo "  export PATH=\"\$HOME/.local/bin:\$PATH\""
echo "  export PATH=\"\$(ruby -e 'puts Gem.user_dir')/bin:\$PATH\""
