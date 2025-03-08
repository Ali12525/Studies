using ShopApp.Services;
using ShopApp.Models;
using Microsoft.Extensions.Configuration;
using Moq;

namespace ShopAppTests.Services
{
    [TestFixture]
    internal class TestsProductService
    {
        private IProductService _productService;
        private Mock<IConfiguration> _mockConfig;
        private Product _testProduct;

        [SetUp]
        public void Setup()
        {
            _mockConfig = new Mock<IConfiguration>();
            _mockConfig.Setup(c => c["DataBaseFilePath"]).Returns("test_db.txt");

            _productService = new ProductService(_mockConfig.Object);
            _testProduct = new Product
            {
                Id = Guid.NewGuid(),
                Description = "Test Product",
                Price = 10.0
            };
        }

        // Tests for Add method
        [TestCase("Product Description", 10)]
        [TestCase("", 15)]
        [TestCase("Product Description", 10.15)]
        public void TestAddMethod_ShouldAddProduct_WhenNew(string description, double price)
        {
            Product product = new Product
            {
                Id = Guid.NewGuid(),
                Description = description,
                Price = price
            };
            var result = _productService.Add(product);
            Assert.That(result, Is.EqualTo(product), "Product was not added correctly");
        }

        [Test]
        public void TestAddMethod_ShouldReturnNull_WhenDuplicate()
        {
            _productService.Add(_testProduct);
            var result = _productService.Add(_testProduct);
            Assert.That(result, Is.Null, "A duplicate product has been added.");
        }

        // Tests for Remove method
        [Test]
        public void TestRemoveMethod_ShouldRemoveProduct_WhenExists()
        {
            _productService.Add(_testProduct);
            var result = _productService.Remove(_testProduct.Id);

            Assert.Multiple(() =>
            {
                Assert.That(result, Is.EqualTo(_testProduct), "Incorrect product returned on removal");
                Assert.That(_productService.Search(_testProduct.Id), Is.Null, "Product still exists after removal");
            });
        }

        [Test]
        public void TestRemoveMethod_ShouldReturnNull_WhenNotExists()
        {
            var result = _productService.Remove(Guid.NewGuid());
            Assert.That(result, Is.Null, "Non-existent product was removed");
        }

        // Tests for Edit method
        [Test]
        public void TestEditMethod_ShouldEditProduct_WhenExists()
        {
            _productService.Add(_testProduct);
            var updatedProduct = new Product
            {
                Id = _testProduct.Id,
                Description = "Updated Product",
                Price = 30.0
            };

            var result = _productService.Edit(updatedProduct);

            Assert.Multiple(() =>
            {
                Assert.That(result, Is.EqualTo(updatedProduct), "Product was not updated");
                var searchedProduct = _productService.Search(updatedProduct.Id);
                Assert.That(searchedProduct?.Description, Is.EqualTo(updatedProduct.Description), "Description was not updated");
                Assert.That(searchedProduct?.Price, Is.EqualTo(updatedProduct.Price), "Price was not updated");
            });
        }

        [Test]
        public void TestEditMethod_ShouldReturnNull_WhenNotExists()
        {
            var result = _productService.Edit(_testProduct);
            Assert.That(result, Is.Null, "Edited non-existent product");
        }

        // Tests for Search method
        [Test]
        public void TestSearchMethod_ShouldFindProduct_WhenExists()
        {
            _productService.Add(_testProduct);
            var result = _productService.Search(_testProduct.Id);
            Assert.That(result, Is.EqualTo(_testProduct), "Product not found");
        }

        [Test]
        public void TestSearchMethod_ShouldReturnNull_WhenNotExists()
        {
            var result = _productService.Search(Guid.NewGuid());
            Assert.That(result, Is.Null, "Non-existent product was found");
        }

        [TearDown]
        public void Cleanup()
        {
            if (File.Exists("test_db.txt"))
            {
                File.Delete("test_db.txt");
            }
        }
    }
}