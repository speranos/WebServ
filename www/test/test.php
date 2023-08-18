<?php
// Set appropriate headers for CGI execution
header("Content-Type: text/html; charset=UTF-8");

// Print the CGI environment variables
echo "<h1>CGI Environment Variables</h1>";

echo "<table>";
foreach ($_SERVER as $key => $value) {
    echo "<tr><td><strong>$key</strong></td><td>$value</td></tr>";
}
echo "</table>";

// Print some dynamic content
echo "<h2>Dynamic Content</h2>";
echo "<p>This page was generated at " . date("Y-m-d H:i:s") . "</p>";
?>
