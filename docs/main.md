# CSE 111 HW 4 -- GitHub CI

## Goals

This homework serves as practice for GitHub CI (continuous integration).

Your final project repository must use Continuous Integration, which is a common industry practice to automatically build and test every revision. Follow this assignment to learn how to properly create a repository for your final project.

Feel free to use the following Repository as an example: <https://github.com/ucsc-cse111/github_setup>

## AI Usage

When using AI LLMs, (such as ChatGPT, Claude, CoPilot, Bard, etc.), ensure careful review of output before submission.

Include citations for all AI-derived content, specifying the model used and providing a link to the conversation history as a comment. An example of an acceptable link is shown here: <https://chat.openai.com/share/b80473dc-a9d9-4435-97a2-e13e41a59c38>. This link was obtained from ChatGPT, by clicking the share icon in the top-right corner of the interaction.

## Tasks

1. Have one person in your group create a GitHub organization and then create a *private* GitHub repository. Then, they should invite everyone else in your group and the course instructors to have Write privileges.

Instructor GitHubs:

* anranlee99
* BenPrincen
* gudhillon
* ryuchannnnn
* sifferman

2. Create a repository named `rom_archive`. Leave it as an empty repository. It will serve as a way for you to receive rom files and documentation specific to your project.

3. Create a CI to build your final project source files. For now, you may just include a basic "Hello, World" or similar.

4. Create a CI to verify the format of your final project source files.

5. Create a CI to release an updated version of a PDF document. This will eventually turn into your project report.

## Submission

You will submit on Canvas: <https://canvas.ucsc.edu/courses/72707/assignments/583982>

Please provide links to the following:

1. A link to your final project repo.
2. A link to you `rom_archive` repo.
3. A link to a **FAILING** action to build your final project source files due to a syntax error.
4. A link to a passing action to build your final project source files.
5. A link to a **FAILING** action to verify the format of your final project source files due to poor formatting.
6. A link to a passing action to verify the format of your final project source files.
7. A link to a passing action that built and released your final project report PDF.
8. A link to the released PDF file.